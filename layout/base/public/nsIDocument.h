/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
#ifndef nsIDocument_h___
#define nsIDocument_h___

#include "nslayout.h"
#include "nsISupports.h"
#include "nsIUnicharInputStream.h"
#include "nsGUIEvent.h"

class nsIArena;
class nsIContent;
class nsIDocumentContainer;
class nsIDocumentObserver;
class nsIPresContext;
class nsIPresShell;
class nsISelection;
class nsIStreamListener;
class nsIStreamObserver;
class nsIStyleSet;
class nsIStyleSheet;
class nsIURL;
class nsIViewManager;
class nsString;
class nsIScriptContextOwner;
class nsIContentViewerContainer;
class nsIDOMEvent;
class nsIDeviceContext;
class nsIParser;
class nsIDOMNode;
class nsXIFConverter;

// IID for the nsIDocument interface
#define NS_IDOCUMENT_IID      \
{ 0x94c6ceb0, 0x9447, 0x11d1, \
  {0x93, 0x23, 0x00, 0x80, 0x5f, 0x8a, 0xdd, 0x32} }

// specification for data to be sent via form "post"
// IID for the nsIPostData interface -  152ab6e0-ff13-11d1-beb9-00805f8a66dc
#define NS_IPOSTDATA_IID      \
{ 0x152ab6e0, 0xff13, 0x11d1, \
  {0xbe, 0xb9, 0x00, 0x80, 0x5f, 0x8a, 0x66, 0xdc} }

class nsIPostData : public nsISupports {
public:
  virtual PRBool       IsFile()  = 0;    // is the data a file (or raw data)
  virtual const char*  GetData() = 0;    // get the file name or raw data
  virtual PRInt32      GetDataLength() = 0;
};

//----------------------------------------------------------------------

// Document interface
class nsIDocument : public nsISupports {
public:
  // All documents have a memory arena associated with them which is
  // used for memory allocation during document creation. This call
  // returns the arena associated with this document.
  virtual nsIArena* GetArena() = 0;

  NS_IMETHOD StartDocumentLoad(nsIURL *aUrl, 
                               nsIContentViewerContainer* aContainer,
                               nsIStreamListener **aDocListener) = 0;

  /**
   * Return the title of the document. May return null.
   */
  virtual const nsString* GetDocumentTitle() const = 0;

  /**
   * Return the URL for the document. May return null.
   */
  virtual nsIURL* GetDocumentURL() const = 0;

  /**
   * Return a standard name for the document's character set. This will
   * trigger a startDocumentLoad if necessary to answer the question.
   */
  virtual nsCharSetID GetDocumentCharacterSet() const = 0;
  virtual void SetDocumentCharacterSet(nsCharSetID aCharSetID) = 0;

  /**
   * Create a new presentation shell that will use aContext for
   * it's presentation context (presentation context's <b>must not</b> be
   * shared among multiple presentation shell's).
   */
  virtual nsresult CreateShell(nsIPresContext* aContext,
                               nsIViewManager* aViewManager,
                               nsIStyleSet* aStyleSet,
                               nsIPresShell** aInstancePtrResult) = 0;
  virtual PRBool DeleteShell(nsIPresShell* aShell) = 0;
  virtual PRInt32 GetNumberOfShells() = 0;
  virtual nsIPresShell* GetShellAt(PRInt32 aIndex) = 0;

  /**
   * Return the parent document of this document. Will return null
   * unless this document is within a compound document and has a parent.
   */
  virtual nsIDocument* GetParentDocument() = 0;
  virtual void SetParentDocument(nsIDocument* aParent) = 0;
  virtual void AddSubDocument(nsIDocument* aSubDoc) = 0;
  virtual PRInt32 GetNumberOfSubDocuments() = 0;
  virtual nsIDocument* GetSubDocumentAt(PRInt32 aIndex) = 0;

  /**
   * Return the root content object for this document.
   */
  virtual nsIContent* GetRootContent() = 0;
  virtual void SetRootContent(nsIContent* aRoot) = 0;

  /**
   * Get the style sheets owned by this document.
   * Style sheets are ordered, most significant first.
   */
  virtual PRInt32 GetNumberOfStyleSheets() = 0;
  virtual nsIStyleSheet* GetStyleSheetAt(PRInt32 aIndex) = 0;
  virtual void AddStyleSheet(nsIStyleSheet* aSheet) = 0;

  /**
   * Set the object from which a document can get a script context.
   * This is the context within which all scripts (during document 
   * creation and during event handling) will run.
   */
  virtual nsIScriptContextOwner *GetScriptContextOwner() = 0;
  virtual void SetScriptContextOwner(nsIScriptContextOwner *aScriptContextOwner) = 0;

  //----------------------------------------------------------------------

  // Document notification API's

  /**
   * Add a new observer of document change notifications. Whenever
   * content is changed, appended, inserted or removed the observers are
   * informed.
   */
  virtual void AddObserver(nsIDocumentObserver* aObserver) = 0;

  /**
   * Remove an observer of document change notifications. This will
   * return false if the observer cannot be found.
   */
  virtual PRBool RemoveObserver(nsIDocumentObserver* aObserver) = 0;

  // Observation hooks used by content nodes to propagate
  // notifications to document observers.
  NS_IMETHOD BeginLoad() = 0;
  NS_IMETHOD EndLoad() = 0;
  virtual void ContentChanged(nsIContent* aContent,
                              nsISupports* aSubContent) = 0;

  virtual void ContentAppended(nsIContent* aContainer) = 0;

  virtual void ContentInserted(nsIContent* aContainer,
                               nsIContent* aChild,
                               PRInt32 aIndexInContainer) = 0;

  virtual void ContentReplaced(nsIContent* aContainer,
                               nsIContent* aOldChild,
                               nsIContent* aNewChild,
                               PRInt32 aIndexInContainer) = 0;

  virtual void ContentWillBeRemoved(nsIContent* aContainer,
                                    nsIContent* aChild,
                                    PRInt32 aIndexInContainer) = 0;

  virtual void ContentHasBeenRemoved(nsIContent* aContainer,
                                     nsIContent* aChild,
                                     PRInt32 aIndexInContainer) = 0;
  /**
    * Returns the Selection Object
   */
  virtual nsISelection * GetSelection() = 0;

  /**
    * Selects all the Content
   */
  virtual void SelectAll() = 0;

  /**
    * Finds text in content
   */
  NS_IMETHOD FindNext(const nsString &aSearchStr, PRBool aMatchCase, PRBool aSearchDown, PRBool &aIsFound) = 0;

  /**
    * Copies all text from the selection
   */
  virtual void GetSelectionText(nsString & aText) = 0;

  /**
    * Converts the document or a selection of the 
    * document to XIF (XML Interchange Format)
    * and places the result in aBuffer.
    
    * NOTE: we may way to place the result in a stream,
    * but we will use a string for now -- gpk
  */
  virtual void CreateXIF(nsString & aBuffer, PRBool aUseSelection) = 0;
  virtual void ToXIF(nsXIFConverter& aConverter, nsIDOMNode* aNode) = 0;
  virtual void BeginConvertToXIF(nsXIFConverter& aConverter, nsIDOMNode* aNode) = 0;
  virtual void ConvertChildrenToXIF(nsXIFConverter& aConverter, nsIDOMNode* aNode) = 0;
  virtual void FinishConvertToXIF(nsXIFConverter& aConverter, nsIDOMNode* aNode) = 0;

  /* Helper methods to help determine the logical positioning of content */
  virtual PRBool IsInRange(const nsIContent *aStartContent, const nsIContent* aEndContent, const nsIContent* aContent) const = 0;
  virtual PRBool IsBefore(const nsIContent *aNewContent, const nsIContent* aCurrentContent) const = 0;
  virtual PRBool IsInSelection(const nsIContent *aContent) const = 0;
  virtual nsIContent* GetPrevContent(const nsIContent *aContent) const = 0;
  virtual nsIContent* GetNextContent(const nsIContent *aContent) const = 0;
  virtual void SetDisplaySelection(PRBool aToggle) = 0;
  virtual PRBool GetDisplaySelection() const = 0;

  NS_IMETHOD HandleDOMEvent(nsIPresContext& aPresContext, 
                            nsEvent* aEvent, 
                            nsIDOMEvent** aDOMEvent,
                            PRUint32 aFlags,
                            nsEventStatus& aEventStatus) = 0;




};


// XXX These belong somewhere else
extern NS_LAYOUT nsresult
   NS_NewHTMLDocument(nsIDocument** aInstancePtrResult);

extern NS_LAYOUT nsresult
   NS_NewImageDocument(nsIDocument** aInstancePtrResult);

// Note: The buffer passed into NewPostData(...) becomes owned by the IPostData
//       instance and is freed when the instance is destroyed...
//
extern NS_LAYOUT nsresult
   NS_NewPostData(PRBool aIsFile, char *aData, nsIPostData** aInstancePtrResult);

#endif /* nsIDocument_h___ */
